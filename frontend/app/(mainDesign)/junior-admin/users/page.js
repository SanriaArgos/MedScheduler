"use client";
import { useState, useEffect } from "react";
import { useRouter } from "next/navigation";
import Link from "next/link";

export default function JuniorAdminUsersPage() {
    const [loading, setLoading] = useState(true);
    const [users, setUsers] = useState([]);
    const [error, setError] = useState("");
    const [searchQuery, setSearchQuery] = useState("");
    const [filteredUsers, setFilteredUsers] = useState([]);
    const router = useRouter();

    useEffect(() => {
        const isLoggedIn = localStorage.getItem('isLoggedIn');
        const userData = JSON.parse(localStorage.getItem('medSchedulerUser') || '{}');

        if (!isLoggedIn || !userData.userId || userData.userType !== 'junior') {
            router.push('/login');
            return;
        }

        fetchUsers();
    }, [router]);

    useEffect(() => {
        if (searchQuery) {
            const lowercaseQuery = searchQuery.toLowerCase();
            const filtered = users.filter(user =>
                user.last_name?.toLowerCase().includes(lowercaseQuery) ||
                user.first_name?.toLowerCase().includes(lowercaseQuery) ||
                user.phone?.includes(searchQuery)
            );
            setFilteredUsers(filtered);
        } else {
            setFilteredUsers(users);
        }
    }, [searchQuery, users]);

    const fetchUsers = async () => {
        try {
            const response = await fetch('https://api.medscheduler.ru/get_users');
            const data = await response.json();

            if (response.ok && data.success) {
                // Фильтруем только пациентов
                const patients = data.users.filter(user => user.type === 'patient');
                setUsers(patients);
                setFilteredUsers(patients);
            } else {
                setError(data.error || "Ошибка при получении списка пользователей");
            }
        } catch (err) {
            console.error("Error fetching users:", err);
            setError("Не удалось подключиться к серверу");
        } finally {
            setLoading(false);
        }
    };

    if (loading) {
        return (
            <div className="min-h-[calc(80vh-theme(space.36))] flex items-center justify-center">
                <div className="text-xl text-gray-600">Загрузка пользователей...</div>
            </div>
        );
    }

    return (
        <div className="max-w-6xl mx-auto py-8 px-4">
            <h1 className="text-3xl font-bold text-main2 mb-8">Пациенты</h1>

            {error && (
                <div className="bg-red-100 border border-red-400 text-red-700 px-4 py-3 rounded mb-4">
                    {error}
                </div>
            )}

            <div className="bg-white shadow-md rounded-lg p-6">
                <div className="mb-6">
                    <input
                        type="text"
                        value={searchQuery}
                        onChange={(e) => setSearchQuery(e.target.value)}
                        placeholder="Поиск по ФИО или телефону"
                        className="w-full px-3 py-2 border border-gray-300 rounded-md"
                    />
                </div>

                {filteredUsers.length === 0 ? (
                    <p className="text-gray-600">Пациенты не найдены</p>
                ) : (
                    <div className="overflow-x-auto">
                        <table className="min-w-full">
                            <thead>
                                <tr className="border-b">
                                    <th className="px-6 py-3 text-left">ФИО</th>
                                    <th className="px-6 py-3 text-left">Телефон</th>
                                    <th className="px-6 py-3 text-left">ID</th>
                                </tr>
                            </thead>
                            <tbody>
                                {filteredUsers.map((user) => (
                                    <tr key={user.user_id} className="border-b hover:bg-gray-50">
                                        <td className="px-6 py-4">
                                            {user.last_name} {user.first_name} {user.patronymic || ''}
                                        </td>
                                        <td className="px-6 py-4">{user.phone}</td>
                                        <td className="px-6 py-4">{user.user_id}</td>
                                    </tr>
                                ))}
                            </tbody>
                        </table>
                    </div>
                )}
            </div>
        </div>
    );
}