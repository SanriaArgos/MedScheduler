"use client";

import { useState, useEffect } from "react";
import { useRouter, useSearchParams } from 'next/navigation';
import Link from "next/link";

export default function SearchPage() {
    const router = useRouter();
    const searchParams = useSearchParams();
    const [loading, setLoading] = useState(true);
    const [doctors, setDoctors] = useState([]);
    const [error, setError] = useState("");
    const [serverDown, setServerDown] = useState(false);

    // Фильтры
    const [regions, setRegions] = useState([]);
    const [selectedRegion, setSelectedRegion] = useState("-");

    const [settlementTypes, setSettlementTypes] = useState([]);
    const [selectedSettlementType, setSelectedSettlementType] = useState("-");

    const [settlementNames, setSettlementNames] = useState([]);
    const [selectedSettlementName, setSelectedSettlementName] = useState("-");

    const [hospitalNames, setHospitalNames] = useState([]);
    const [selectedHospital, setSelectedHospital] = useState("-");

    const [specialties, setSpecialties] = useState([]);
    const [selectedSpecialty, setSelectedSpecialty] = useState("-");

    const [sortByRating, setSortByRating] = useState(false);

    // Состояние для загрузки врачей
    const [loadingDoctors, setLoadingDoctors] = useState(false);
    const [initialQueryApplied, setInitialQueryApplied] = useState(false);

    useEffect(() => {
        // Получение query параметра из URL
        const query = searchParams.get('query');

        // Загрузка фильтров при первом рендере
        const fetchFilters = async () => {
            try {
                // Загрузка регионов
                const regionsResponse = await fetch('https://api.medscheduler.ru/get_regions');
                const regionsData = await regionsResponse.json();
                if (regionsResponse.ok && regionsData.success) {
                    setRegions(regionsData.regions);
                }

                // Загрузка типов населенных пунктов
                const settlementTypesResponse = await fetch('https://api.medscheduler.ru/get_settlement_types');
                const settlementTypesData = await settlementTypesResponse.json();
                if (settlementTypesResponse.ok && settlementTypesData.success) {
                    setSettlementTypes(settlementTypesData.settlement_types);
                }

                // Загрузка названий населенных пунктов
                const settlementNamesResponse = await fetch('https://api.medscheduler.ru/get_settlement_names');
                const settlementNamesData = await settlementNamesResponse.json();
                if (settlementNamesResponse.ok && settlementNamesData.success) {
                    setSettlementNames(settlementNamesData.settlement_names);
                }

                // Загрузка названий больниц
                const hospitalNamesResponse = await fetch('https://api.medscheduler.ru/get_hospital_full_names');
                const hospitalNamesData = await hospitalNamesResponse.json();
                if (hospitalNamesResponse.ok && hospitalNamesData.success) {
                    setHospitalNames(hospitalNamesData.hospital_full_names);
                }

                // Загрузка специальностей
                const specialtiesResponse = await fetch('https://api.medscheduler.ru/get_specialties');
                const specialtiesData = await specialtiesResponse.json();
                if (specialtiesResponse.ok && specialtiesData.success) {
                    setSpecialties(specialtiesData.specialties);
                }

                setLoading(false);

                // Если есть поисковый запрос, применяем его к специальности или названию больницы
                if (query && !initialQueryApplied) {
                    // Проверяем, совпадает ли запрос с какой-либо специальностью
                    const matchedSpecialty = specialtiesData.specialties.find(
                        spec => spec.toLowerCase().includes(query.toLowerCase())
                    );

                    // Проверяем, совпадает ли запрос с каким-либо названием больницы
                    const matchedHospital = hospitalNamesData.hospital_full_names.find(
                        hospital => hospital.toLowerCase().includes(query.toLowerCase())
                    );

                    if (matchedSpecialty) {
                        setSelectedSpecialty(matchedSpecialty);
                    } else if (matchedHospital) {
                        setSelectedHospital(matchedHospital);
                    }

                    // Выполняем поиск с примененными фильтрами
                    setTimeout(() => {
                        handleSearch();
                        setInitialQueryApplied(true);
                    }, 500);
                } else {
                    // Автоматически запускаем поиск сразу после загрузки фильтров, даже если нет поискового запроса
                    setTimeout(() => {
                        handleSearch();
                    }, 500);
                }

            } catch (err) {
                console.error("Error fetching filters:", err);
                setError("Не удалось загрузить фильтры. Проверьте соединение с сервером.");
                setLoading(false);
            }
        };

        fetchFilters();
    }, [searchParams]);

    const searchDoctors = async (filters) => {
        setLoadingDoctors(true);
        setError("");
        setServerDown(false);

        try {
            const response = await fetch('https://api.medscheduler.ru/search_doctors', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify(filters)
            });
            
            const data = await response.json();

            if (response.ok && data.success) {
                // Преобразуем данные для совместимости с интерфейсом
                const formattedDoctors = data.doctors.map(doc => ({
                    doctor_id: doc.doctor_id || 0,
                    last_name: doc.fio?.split(' ')[0] || '',
                    first_name: doc.fio?.split(' ')[1] || '',
                    patronymic: doc.fio?.split(' ')[2] || '',
                    specialty: doc.specialty || '',
                    experience: doc.experience || 0,
                    price: doc.price || 0,
                    average_rating: doc.average_rate || 0,
                    hospitals: [{ full_name: doc.hospital_name || "Клиника не указана" }]
                }));
                setDoctors(formattedDoctors);
            } else {
                setError(data.error || "Ошибка при поиске врачей");
                setDoctors([]);
            }
        } catch (err) {
            console.error("Error searching doctors:", err);
            
            if (err.message === 'Failed to fetch' || !window.navigator.onLine) {
                setServerDown(true);
                setError("Сервер временно недоступен. Пожалуйста, повторите попытку позже.");
            } else {
                setError("Не удалось выполнить поиск. Проверьте соединение с сервером.");
            }
            
            setDoctors([]);
        } finally {
            setLoadingDoctors(false);
        }
    };

    const handleSearch = () => {
        const filters = {
            region: selectedRegion,
            settlement_type: selectedSettlementType,
            settlement_name: selectedSettlementName,
            full_name: selectedHospital,
            specialty: selectedSpecialty,
            sort_by_rating: sortByRating
        };

        searchDoctors(filters);
    };

    const handleReset = () => {
        setSelectedRegion("-");
        setSelectedSettlementType("-");
        setSelectedSettlementName("-");
        setSelectedHospital("-");
        setSelectedSpecialty("-");
        setSortByRating(false);

        setDoctors([]);
        setError("");
    };

    if (loading) {
        return (
            <div className="min-h-[calc(80vh-theme(space.36))] flex items-center justify-center">
                <div className="text-xl text-gray-600">Загрузка фильтров...</div>
            </div>
        );
    }

    return (
        <div className="max-w-6xl mx-auto py-8 px-4">
            <h1 className="text-3xl font-bold text-main2 mb-8">Поиск врачей</h1>

            {error && (
                <div className="bg-red-100 border border-red-400 text-red-700 px-4 py-3 rounded mb-6">
                    <div className="flex flex-col">
                        <p>{error}</p>
                        {serverDown && (
                            <div className="flex flex-col gap-2 mt-2">
                                <button 
                                    onClick={handleSearch}
                                    className="px-4 py-2 bg-red-600 text-white rounded hover:bg-red-700 w-fit"
                                >
                                    Повторить попытку
                                </button>
                            </div>
                        )}
                    </div>
                </div>
            )}

            <div className="grid grid-cols-1 lg:grid-cols-4 gap-6">
                {/* Фильтры - боковая панель */}
                <div className="lg:col-span-1">
                    <div className="bg-white shadow-md rounded-lg p-6">
                        <h2 className="text-xl font-semibold text-main2 mb-4">Фильтры</h2>

                        <div className="space-y-4">
                            <div>
                                <label htmlFor="region" className="block text-sm font-medium text-gray-700 mb-1">
                                    Регион
                                </label>
                                <select
                                    id="region"
                                    value={selectedRegion}
                                    onChange={(e) => setSelectedRegion(e.target.value)}
                                    className="w-full px-3 py-2 border border-gray-300 rounded-md focus:outline-none focus:ring-1 focus:ring-main"
                                >
                                    {regions.map((region, index) => (
                                        <option key={index} value={region}>{region}</option>
                                    ))}
                                </select>
                            </div>

                            <div>
                                <label htmlFor="settlementType" className="block text-sm font-medium text-gray-700 mb-1">
                                    Тип населенного пункта
                                </label>
                                <select
                                    id="settlementType"
                                    value={selectedSettlementType}
                                    onChange={(e) => setSelectedSettlementType(e.target.value)}
                                    className="w-full px-3 py-2 border border-gray-300 rounded-md focus:outline-none focus:ring-1 focus:ring-main"
                                >
                                    {settlementTypes.map((type, index) => (
                                        <option key={index} value={type}>{type}</option>
                                    ))}
                                </select>
                            </div>

                            <div>
                                <label htmlFor="settlementName" className="block text-sm font-medium text-gray-700 mb-1">
                                    Название населенного пункта
                                </label>
                                <select
                                    id="settlementName"
                                    value={selectedSettlementName}
                                    onChange={(e) => setSelectedSettlementName(e.target.value)}
                                    className="w-full px-3 py-2 border border-gray-300 rounded-md focus:outline-none focus:ring-1 focus:ring-main"
                                >
                                    {settlementNames.map((name, index) => (
                                        <option key={index} value={name}>{name}</option>
                                    ))}
                                </select>
                            </div>

                            <div>
                                <label htmlFor="hospital" className="block text-sm font-medium text-gray-700 mb-1">
                                    Клиника
                                </label>
                                <select
                                    id="hospital"
                                    value={selectedHospital}
                                    onChange={(e) => setSelectedHospital(e.target.value)}
                                    className="w-full px-3 py-2 border border-gray-300 rounded-md focus:outline-none focus:ring-1 focus:ring-main"
                                >
                                    {hospitalNames.map((name, index) => (
                                        <option key={index} value={name}>{name}</option>
                                    ))}
                                </select>
                            </div>

                            <div>
                                <label htmlFor="specialty" className="block text-sm font-medium text-gray-700 mb-1">
                                    Специальность
                                </label>
                                <select
                                    id="specialty"
                                    value={selectedSpecialty}
                                    onChange={(e) => setSelectedSpecialty(e.target.value)}
                                    className="w-full px-3 py-2 border border-gray-300 rounded-md focus:outline-none focus:ring-1 focus:ring-main"
                                >
                                    {specialties.map((specialty, index) => (
                                        <option key={index} value={specialty}>{specialty}</option>
                                    ))}
                                </select>
                            </div>

                            <div className="flex items-center">
                                <input
                                    id="sortByRating"
                                    type="checkbox"
                                    checked={sortByRating}
                                    onChange={(e) => setSortByRating(e.target.checked)}
                                    className="h-4 w-4 text-main focus:ring-main border-gray-300 rounded"
                                />
                                <label htmlFor="sortByRating" className="ml-2 block text-sm text-gray-700">
                                    Сортировать по рейтингу
                                </label>
                            </div>

                            <div className="pt-4 flex gap-2">
                                <button
                                    onClick={handleSearch}
                                    disabled={loadingDoctors}
                                    className="w-full px-4 py-2 bg-main text-white rounded hover:bg-main2 transition-colors disabled:opacity-50"
                                >
                                    {loadingDoctors ? "Поиск..." : "Найти"}
                                </button>

                                <button
                                    onClick={handleReset}
                                    className="w-full px-4 py-2 bg-gray-200 text-gray-800 rounded hover:bg-gray-300 transition-colors"
                                >
                                    Сбросить
                                </button>
                            </div>
                        </div>
                    </div>
                </div>

                {/* Результаты поиска */}
                <div className="lg:col-span-3">
                    {loadingDoctors ? (
                        <div className="flex justify-center items-center min-h-[400px]">
                            <div className="text-xl text-gray-600">Загрузка врачей...</div>
                        </div>
                    ) : doctors.length === 0 && !error ? (
                        <div className="bg-white shadow-md rounded-lg p-8 text-center">
                            <p className="text-lg text-gray-600 mb-4">Выберите фильтры и нажмите "Найти"</p>
                            <p className="text-gray-500">Результаты поиска появятся здесь</p>
                        </div>
                    ) : doctors.length === 0 && error ? (
                        <div className="bg-white shadow-md rounded-lg p-8 text-center">
                            <p className="text-lg text-gray-600 mb-4">По вашему запросу врачи не найдены</p>
                            <p className="text-gray-500">Попробуйте изменить параметры поиска</p>
                        </div>
                    ) : (
                        <div className="grid grid-cols-1 md:grid-cols-2 gap-4">
                            {doctors.map((doctor) => (
                                <div key={doctor.doctor_id} className="bg-white shadow-md rounded-lg overflow-hidden hover:shadow-lg transition-shadow">
                                    <div className="p-5">
                                        <Link href={`/doctor/${doctor.doctor_id}`} className="block">
                                            <h3 className="text-lg font-semibold text-main2 hover:text-main">
                                                {doctor.last_name} {doctor.first_name} {doctor.patronymic}
                                            </h3>
                                        </Link>
                                        <p className="text-sm text-gray-600 mt-1">
                                            <span className="font-medium">{doctor.specialty}</span>
                                        </p>

                                        <div className="mt-3 flex items-center">
                                            <div className="flex items-center">
                                                <svg xmlns="http://www.w3.org/2000/svg" className="h-5 w-5 text-yellow-500" viewBox="0 0 20 20" fill="currentColor">
                                                    <path d="M9.049 2.927c.3-.921 1.603-.921 1.902 0l1.07 3.292a1 1 0 00.95.69h3.462c.969 0 1.371 1.24.588 1.81l-2.8 2.034a1 1 0 00-.364 1.118l1.07 3.292c.3.921-.755 1.688-1.54 1.118l-2.8-2.034a1 1 0 00-1.175 0l-2.8 2.034c-.784.57-1.838-.197-1.539-1.118l1.07-3.292a1 1 0 00-.364-1.118L2.98 8.72c-.783-.57-.38-1.81.588-1.81h3.461a1 1 0 00.951-.69l1.07-3.292z" />
                                                </svg>
                                                <span className="ml-1 text-sm font-medium text-gray-700">
                                                    {doctor.average_rating ? doctor.average_rating.toFixed(1) : "Нет отзывов"}
                                                </span>
                                            </div>
                                            <span className="mx-2 text-gray-300">|</span>
                                            <span className="text-sm text-gray-600">
                                                Опыт: {doctor.experience} {doctor.experience === 1 ? "год" : doctor.experience < 5 ? "года" : "лет"}
                                            </span>
                                        </div>

                                        <div className="mt-4 flex justify-between items-center">
                                            <div className="text-sm">
                                                {doctor.hospitals && doctor.hospitals.length > 0 ? (
                                                    <span className="text-gray-600">{doctor.hospitals[0].full_name}</span>
                                                ) : (
                                                    <span className="text-gray-400">Клиника не указана</span>
                                                )}
                                            </div>
                                            <div className="text-main font-medium">
                                                {doctor.price} ₽
                                            </div>
                                        </div>

                                        <div className="mt-4">
                                            <Link
                                                href={`/doctor/${doctor.doctor_id}`}
                                                className="w-full block text-center px-4 py-2 bg-main text-white rounded hover:bg-main2 transition-colors"
                                            >
                                                Подробнее
                                            </Link>
                                        </div>
                                    </div>
                                </div>
                            ))}
                        </div>
                    )}
                </div>
            </div>
        </div>
    );
}
